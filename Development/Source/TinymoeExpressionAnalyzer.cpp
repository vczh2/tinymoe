#include "TinymoeExpressionAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	GrammarFragment
	*************************************************************/

	GrammarFragment::GrammarFragment(GrammarFragmentType _type)
		:type(_type)
	{
	}

	/*************************************************************
	GrammarSymbol
	*************************************************************/

	GrammarSymbol::GrammarSymbol(GrammarSymbolType _type, GrammarSymbolTarget _target)
		:type(_type)
		, target(_target)
	{
	}

	void GrammarSymbol::CalculateUniqueId()
	{
		uniqueId = "";
		for (auto fragment : fragments)
		{
			switch (fragment->type)
			{
			case GrammarFragmentType::Type:
				uniqueId += "<type> ";
				break;
			case GrammarFragmentType::Primitive:
				uniqueId += "<primitive> ";
				break;
			case GrammarFragmentType::Expression:
				uniqueId += "<expression> ";
				break;
			case GrammarFragmentType::List:
				uniqueId += "<list> ";
				break;
			case GrammarFragmentType::Assignable:
				uniqueId += "<assignable> ";
				break;
			case GrammarFragmentType::Argument:
				uniqueId += "<argument> ";
				break;
			case GrammarFragmentType::Name:
				for (auto name : fragment->identifiers)
				{
					uniqueId += name + " ";
				}
				break;
			}
		}
	}

	/*************************************************************
	GrammarStackItem
	*************************************************************/

	GrammarSymbol::Ptr operator+(GrammarSymbol::Ptr symbol, const string& name)
	{
		GrammarFragment::Ptr fragment;
		if (symbol->fragments.size() == 0 || (fragment = symbol->fragments.back())->type != GrammarFragmentType::Name)
		{
			fragment = make_shared<GrammarFragment>(GrammarFragmentType::Name);
			symbol->fragments.push_back(fragment);
		}

		fragment->identifiers.push_back(name);
		return symbol;
	}

	GrammarSymbol::Ptr operator+(GrammarSymbol::Ptr symbol, GrammarFragmentType type)
	{
		auto fragment = make_shared<GrammarFragment>(type);
		symbol->fragments.push_back(fragment);
		return symbol;
	}

	void GrammarStackItem::FillPredefinedSymbols()
	{
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Array)
			+ "array"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::String)
			+ "string"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Integer)
			+ "integer"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Float)
			+ "float"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Symbol)
			+ "symbol"
			);
		
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::True)
			+ "true"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::False)
			+ "false"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::Null)
			+ "null"
			);

		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewType)
			+ "new" + GrammarFragmentType::Type
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewArray)
			+ "new" + "array" + "of" + GrammarFragmentType::Expression + "items"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetArrayItem)
			+ "item" + GrammarFragmentType::Expression + "of" + "array" + GrammarFragmentType::Primitive
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::Invoke)
			+ "invoke" + GrammarFragmentType::Primitive
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::InvokeWith)
			+ "invoke" + GrammarFragmentType::Expression + "with" + GrammarFragmentType::List
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::IsType)
			+ GrammarFragmentType::Primitive + "is" + GrammarFragmentType::Type
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::IsNotType)
			+ GrammarFragmentType::Primitive + "is" + "not" + GrammarFragmentType::Type
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetField)
			+ "field" + GrammarFragmentType::Argument + "of" + GrammarFragmentType::Primitive
			);

		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::End)
			+ "end"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Select)
			+ "select" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Case)
			+ "case" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::TailCall)
			+ "tail" + "call" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::RedirectTo)
			+ "redirect" + "to" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Assign)
			+ "set" + GrammarFragmentType::Assignable + "to" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetArrayItem)
			+ "set" + "item" + GrammarFragmentType::Expression + "of" + "array" + GrammarFragmentType::Expression + "to" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetField)
			+ "set" + "field" + GrammarFragmentType::Argument + "of" + GrammarFragmentType::Expression + "to" + GrammarFragmentType::Expression
			);
	}

	/*************************************************************
	Expression::ToLog
	*************************************************************/

	string LiteralExpression::ToLog()
	{
		if (token.type == CodeTokenType::String)
		{
			return "\"" + CodeToken::EscapeString(token.value) + "\"";
		}
		else
		{
			return token.value;
		}
	}

	string ArgumentExpression::ToLog()
	{
		string result = "<argument>(";
		for (auto token : tokens)
		{
			result += token.value + " ";
		}
		result += ")";
		return result;
	}

	string ReferenceExpression::ToLog()
	{
		return symbol->uniqueId;
	}

	string InvokeExpression::ToLog()
	{
		string result = function->ToLog() + "(";
		for (auto argument : arguments)
		{
			result += argument->ToLog();
			if (argument != arguments.back())
			{
				result += ", ";
			}
		}
		result += ")";
		return result;
	}

	string ListExpression::ToLog()
	{
		string result = "(";
		for (auto element : elements)
		{
			result += element->ToLog();
			if (element != elements.back())
			{
				result += ", ";
			}
		}
		result += ")";
		return result;
	}

	string UnaryExpression::ToLog()
	{
		string result;
		switch (op)
		{
		case UnaryOperator::Positive:
			result = "+ (";
			break;
		case UnaryOperator::Negative:
			result = "- (";
			break;
		case UnaryOperator::Not:
			result = "not (";
			break;
		}
		result += operand->ToLog() + ")";
		return result;
	}

	string BinaryExpression::ToLog()
	{
		string result = "(" + first->ToLog() + " ";
		switch (op)
		{
		case BinaryOperator::Concat:
			result += "&";
			break;
		case BinaryOperator::Add:
			result += "+";
			break;
		case BinaryOperator::Sub:
			result += "-";
			break;
		case BinaryOperator::Mul:
			result += "*";
			break;
		case BinaryOperator::Div:
			result += "/";
			break;
		case BinaryOperator::LT:
			result += "<";
			break;
		case BinaryOperator::GT:
			result += ">";
			break;
		case BinaryOperator::LE:
			result += "<=";
			break;
		case BinaryOperator::GE:
			result += ">=";
			break;
		case BinaryOperator::EQ:
			result += "=";
			break;
		case BinaryOperator::NE:
			result += "<>";
			break;
		case BinaryOperator::And:
			result += "and";
			break;
		case BinaryOperator::Or:
			result += "or";
			break;
		}
		result += " " + second->ToLog() + ")";
		return result;
	}

	/*************************************************************
	Expression::ToCode
	*************************************************************/

	string LiteralExpression::ToCode()
	{
		if (token.type == CodeTokenType::String)
		{
			return "\"" + CodeToken::EscapeString(token.value) + "\"";
		}
		else
		{
			return token.value;
		}
	}

	string ArgumentExpression::ToCode()
	{
		string result = "(";
		for (auto token : tokens)
		{
			result += token.value + " ";
		}
		result += ")";
		return result;
	}

	string ReferenceExpression::ToCode()
	{
		return "(" + symbol->uniqueId + ")";
	}

	string InvokeExpression::ToCode()
	{
		auto reference = dynamic_pointer_cast<ReferenceExpression>(function);
		switch (reference->symbol->type)
		{
		case GrammarSymbolType::Phrase:
		case GrammarSymbolType::Sentence:
		case GrammarSymbolType::Block:
			{
				string result;
				auto it = arguments.begin();
				for (auto fragment : reference->symbol->fragments)
				{
					if (fragment->type == GrammarFragmentType::Name)
					{
						for (auto id : fragment->identifiers)
						{
							result += id + " ";
						}
					}
					else
					{
						result += (*it++)->ToCode() + " ";
					}
				}
				return result;
			}
		}

		string result = "invoke " + function->ToCode() + " with (";
		for (auto argument : arguments)
		{
			result += argument->ToCode();
			if (argument != arguments.back())
			{
				result += ", ";
			}
		}
		result += ")";
		return result;
	}

	string ListExpression::ToCode()
	{
		string result = "(";
		for (auto element : elements)
		{
			result += element->ToCode();
			if (element != elements.back())
			{
				result += ", ";
			}
		}
		result += ")";
		return result;
	}

	string UnaryExpression::ToCode()
	{
		string result;
		switch (op)
		{
		case UnaryOperator::Positive:
			result = "+ (";
			break;
		case UnaryOperator::Negative:
			result = "- (";
			break;
		case UnaryOperator::Not:
			result = "not (";
			break;
		}
		result += operand->ToCode() + ")";
		return result;
	}

	string BinaryExpression::ToCode()
	{
		string result = "(" + first->ToCode() + " ";
		switch (op)
		{
		case BinaryOperator::Concat:
			result += "&";
			break;
		case BinaryOperator::Add:
			result += "+";
			break;
		case BinaryOperator::Sub:
			result += "-";
			break;
		case BinaryOperator::Mul:
			result += "*";
			break;
		case BinaryOperator::Div:
			result += "/";
			break;
		case BinaryOperator::LT:
			result += "<";
			break;
		case BinaryOperator::GT:
			result += ">";
			break;
		case BinaryOperator::LE:
			result += "<=";
			break;
		case BinaryOperator::GE:
			result += ">=";
			break;
		case BinaryOperator::EQ:
			result += "=";
			break;
		case BinaryOperator::NE:
			result += "<>";
			break;
		case BinaryOperator::And:
			result += "and";
			break;
		case BinaryOperator::Or:
			result += "or";
			break;
		}
		result += " " + second->ToCode() + ")";
		return result;
	}

	/*************************************************************
	GrammarStack
	*************************************************************/

	void GrammarStack::Push(GrammarStackItem::Ptr stackItem)
	{
		for (auto symbol : stackItem->symbols)
		{
			symbol->CalculateUniqueId();
		}

		stackItems.push_back(stackItem);
		for (auto symbol : stackItem->symbols)
		{
			availableSymbols.insert(make_pair(symbol->uniqueId, symbol));
		}
	}

	GrammarStackItem::Ptr GrammarStack::Pop()
	{
		auto stackItem = stackItems.back();
		stackItems.pop_back();

		for (auto symbol : stackItem->symbols)
		{
			auto begin = availableSymbols.lower_bound(symbol->uniqueId);
			auto end = availableSymbols.upper_bound(symbol->uniqueId);
			auto it = find_if(begin, end, [symbol](pair<string, GrammarSymbol::Ptr> x){return x.second == symbol; });
			while (it != availableSymbols.end())
			{
				if (it->second == symbol) break;
				it++;
			}
			availableSymbols.erase(it);
		}

		return stackItem;
	}

	CodeError GrammarStack::SuccessError()
	{
		return CodeError();
	}

	CodeError GrammarStack::ParseToken(const string& token, Iterator input, Iterator end, vector<Iterator>& result)
	{
		if (input == end)
		{
			auto token = *(input - 1);
			CodeError error = {
				token,
				token,
				"Unexpected end of line.",
			};
			return error;
		}
		else if (input->value != token)
		{
			auto token = *input;
			CodeError error = {
				token,
				token,
				"\"" + token.value + "\" expected but \"" + input->value + "\" found.",
			};
			return error;
		}
		else
		{
			result.push_back(++input);
			return CodeError();
		}
	}

	CodeError GrammarStack::PickError(CodeError::List& errors)
	{
		auto it = errors.begin();
		auto token = *it++;
		while (it != errors.end())
		{
			if (it->begin.column > token.begin.column)
			{
				token = *it;
			}
			it++;
		}
		return token;
	}

	CodeError GrammarStack::ParseGrammarFragment(GrammarFragment::Ptr fragment, Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		switch (fragment->type)
		{
		case GrammarFragmentType::Type:
			return ParseType(input, end, result);
		case GrammarFragmentType::Primitive:
			return ParsePrimitive(input, end, result);
		case GrammarFragmentType::Expression:
			return ParseExpression(input, end, result);
		case GrammarFragmentType::List:
			return ParseList(input, end, result);
		case GrammarFragmentType::Assignable:
			return ParseAssignable(input, end, result);
		case GrammarFragmentType::Argument:
			return ParseArgument(input, end, result);
		}

		vector<Iterator> nameResult;
		for (auto id : fragment->identifiers)
		{
			auto error = ParseToken(id, input, end, nameResult);
			if (nameResult.size() == 0)
			{
				return error;
			}
			else
			{
				input = nameResult[0];
				nameResult.clear();
			}
		}

		result.push_back(make_pair(input, Expression::Ptr(nullptr)));
		return SuccessError();
	}

	CodeError GrammarStack::ParseGrammarSymbol(GrammarSymbol::Ptr symbol, Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseType(Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParsePrimitive(Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseExpression(Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseList(Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseAssignable(Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseArgument(Iterator input, Iterator end, vector<pair<Iterator, Expression::Ptr>>& result)
	{
		throw 0;
	}
}