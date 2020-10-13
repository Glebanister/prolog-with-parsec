/*
    Parsnip Parser Libary
	http://parsnip-parser.sf.net
	Copyright 2007 Alex Rubinsteyn
	-----------------------------------------------------------------------
	This file is part of Parsnip.

    Parsnip is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Parsnip is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Parsnip.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSNIP_OPTABLE_H
#define PARSNIP_OPTABLE_H

#include <stack>

#include "Parser.h"
#include "StringParser.h"
#include "Whitespace.h"

namespace Parsnip
{

template <typename Input, typename TermType>
struct OpTableParser : public IParser<Input, TermType>
{

    typedef TermType (*ReduceFn)(TermType, TermType);
    typedef TermType (*LabeledReduceFn)(const std::string &, TermType, TermType);
    typedef unsigned OpId;

    enum AssocType
    {
        LEFT,
        RIGHT
    };
    enum FnType
    {
        LABELED,
        UNLABELED
    };

    OpTableParser(ptr<IParser<Input, TermType>> term) : term_parser(term), op_parser(0), num_ops(0)
    {
        this->setName("op_table");
    }

    OpTableParser<Input, TermType> *infix_left(const std::string &op_str, unsigned precedence, ReduceFn reducer)
    {
        add_op(op_str, num_ops, precedence, LEFT, UNLABELED);
        reducers[num_ops] = reducer;

        ++num_ops;
        return this;
    }

    OpTableParser<Input, TermType> *infix_left(const std::string &op_str, unsigned precedence, LabeledReduceFn reducer)
    {

        add_op(op_str, num_ops, precedence, LEFT, LABELED);
        labeled_reducers[num_ops] = reducer;
        ++num_ops;
        return this;
    }

    OpTableParser<Input, TermType> *infix_right(const std::string &op_str, unsigned precedence, ReduceFn reducer)
    {
        add_op(op_str, num_ops, precedence, RIGHT, UNLABELED);
        reducers[num_ops] = reducer;
        ++num_ops;
        return this;
    }

    OpTableParser<Input, TermType> *infix_right(const std::string &op_str, unsigned precedence, LabeledReduceFn reducer)
    {
        add_op(op_str, num_ops, precedence, RIGHT, LABELED);
        labeled_reducers[num_ops] = reducer;
        ++num_ops;
        return this;
    }

protected:
    Result<std::string> parse_op()
    {

        whitespace->parse();
        Result<std::string> result = op_parser->parse();
        whitespace->parse();
        return result;
    }

    virtual Result<TermType> eval()
    {
        Result<TermType> maybe_term;
        Result<std::string> maybe_next_op;
        TermType term;
        unsigned precedence;
        AssocType assoc;
        typename Reader<Input>::IndexT pos;

        //parse first term
        maybe_term = term_parser->parse();

        if (!maybe_term)
        {
            return Result<TermType>::fail();
        }

        term_stack.push(maybe_term.data());

        while (true)
        {
            pos = Reader<Input>::pos();

            maybe_next_op = parse_op();

            if (!maybe_next_op)
            {
                Reader<Input>::set_pos(pos);
                break;
            }

            //parse right-hand-side term
            maybe_term = term_parser->parse();

            if (!maybe_term)
            {
                return Result<TermType>::fail();
            }

            OpId next_op = op_ids[maybe_next_op.data()];

            precedence = precs[next_op];
            assoc = assocs[next_op];

            while (!op_stack.empty() &&
                   ((assoc == LEFT && precedence <= top_precedence) ||
                    (assoc == RIGHT && precedence < top_precedence)))
            {
                reduce_stack();
            }

            term_stack.push(maybe_term.data());

            op_stack.push(next_op);

            top_id = next_op;
            top_precedence = precedence;
            top_assoc = assoc;
        }

        //if no operators were parsed, then we didn't properly parse our expressions
        if (op_stack.size() < 1)
        {
            return Result<TermType>::fail();
        }

        while (!op_stack.empty())
        {
            reduce_stack();
        }

        term = term_stack.top();
        term_stack.pop();
        return Result<TermType>::succeed(term);
    }

private:
    unsigned num_ops;

    ptr<IParser<Input, TermType>> term_parser;
    ptr<IParser<std::string, std::string>> op_parser;

    std::map<std::string, OpId> op_ids;
    std::map<OpId, std::string> op_strings;

    std::map<OpId, unsigned> precs;
    std::map<OpId, AssocType> assocs;
    std::map<OpId, ReduceFn> reducers;
    std::map<OpId, LabeledReduceFn> labeled_reducers;
    std::map<OpId, FnType> fn_type;

    std::stack<TermType> term_stack;
    std::stack<OpId> op_stack;

    AssocType top_assoc;
    unsigned top_precedence;

    OpId top_id;

    void add_op(const std::string &op_str, unsigned id, unsigned precedence, AssocType assoc, FnType fn)
    {
        op_ids[op_str] = id;
        op_strings[id] = op_str;

        precs[id] = precedence;
        assocs[id] = assoc;
        fn_type[id] = fn;

        if (op_parser)
        {
            op_parser = longer(op_parser, str(op_str));
        }
        else
        {
            op_parser = str(op_str);
        }
    }

    void reduce_stack()
    {
        TermType term = term_stack.top();
        term_stack.pop();

        if (fn_type[top_id] == LABELED)
        {
            term = labeled_reducers[top_id](op_strings[top_id], term_stack.top(), term);
        }
        else
        {
            term = reducers[top_id](term_stack.top(), term);
        }

        term_stack.pop();
        term_stack.push(term);

        pop_op();
    }

    void pop_op()
    {
        op_stack.pop();

        if (!op_stack.empty())
        {
            top_id = op_stack.top();
            top_precedence = precs[top_id];
            top_assoc = assocs[top_id];
        }
    }
};

template <typename TermType>
inline OpTableParser<std::string, TermType> *op_table(ptr<IParser<std::string, TermType>> term_parser)
{
    return new OpTableParser<std::string, TermType>(term_parser);
}
} // namespace Parsnip

#endif
