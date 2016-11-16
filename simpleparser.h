/**
 * @brief The SimpleParser class
 *
 * # Grammar:
 *     * PrimaryExpression:
 *         * Value
 *         * Data
 *         * ( Expression )
 *     * PostFixExpression:
 *         * PrimaryExpression
 *     * UnaryExpression:
 *         * PostFixExpression
 *         * ++ UnaryExpression
 *         * -- UnaryExpression
 *         * UnaryOperator CastExpression
 *     * UnaryOperator:
 *         * +
 *         * -
 *         * ~
 *         * !
 *     * CastExpression:
 *         * UnaryExpression
 *         * (TypeName) CastExpression
 *     * TypeName:
 *         * Integer
 *         * Double
 *         * Bool
 *         * String
 *     * MultiplicativeExpression:
 *         * CastExpression
 *         * MultiplicativeExpression * CastExpression
 *         * MultiplicativeExpression / CastExpression
 *         * MultiplicativeExpression % CastExpression
 *     * AdditiveExpression:
 *         * MultiplicativeExpression
 *         * AdditiveExpression + MultiplicativeExpression
 *         * AdditiveExpression - MultiplicativeExpression
 *     * ShiftExpression:
 *         * AdditiveExpression
 *         * ShiftExpression << AdditiveExpression
 *         * ShiftExpression >> AdditiveExpression
 *     * RelationalExpression:
 *         * ShiftExpression
 *         * RelationalExpression < ShiftExpression
 *         * RelationalExpression > ShiftExpression
 *         * RelationalExpression <= ShiftExpression
 *         * RelationalExpression >= ShiftExpression
 *     * EqualityExpression:
 *         * RelationalExpression
 *         * EqualityExpression == RelationalExpression
 *         * EqualityExpression != RelationalExpression
 *     * BitwiseANDExpression:
 *         * EqualityExpression
 *         * BitwiseANDExpression & EqualityExpression
 *     * BitwiseXORExpression:
 *         * BitwiseANDExpression
 *         * BitwiseXORExpression ^ BitwiseANDExpression
 *     * BitwiseORExpression:
 *         * BitwiseXORExpression
 *         * BitwiseORExpression | BitwiseXORExpression
 *     * LogicalANDExpression:
 *         * BitwiseORExpression
 *         * LogicalANDExpression && BitwiseORExpression
 *     * LogicalXORExpression:
 *         * LogicalANDExpression
 *         * LogicalXORExpression ^^ LogicalANDExpression
 *     * LogicalORExpression:
 *         * LogicalXORExpression
 *         * LogicalORExpression || LogicalXORExpression
 *     * ConditionalExpression:
 *         * LogicalORExpression
 *         * LogicalORExpression ? Expression : ConditionalExpression
 *     * Expresion:
 *         * ConditionalExpresion
 *
 * ## Rules:
 *     Expresion                :   ConditionalExpresion
 *     ConditionalExpression    :   LogicalORExpression         ( ? Expression : LogicalORExpression )*
 *     LogicalORExpression      :   LogicalXORExpression        ( || LogicalXORExpression )*
 *     LogicalXORExpression     :   LogicalANDExpression        ( ^^ LogicalANDExpression )*
 *     LogicalANDExpression     :   BitwiseORExpression         ( && BitwiseORExpression )*
 *     BitwiseORExpression      :   BitwiseXORExpression        ( | BitwiseXORExpression )*
 *     BitwiseXORExpression     :   BitwiseANDExpression        ( ^ BitwiseANDExpression )*
 *     BitwiseANDExpression     :   EqualityExpression          ( & EqualityExpression )*
 *     EqualityExpression       :   RelationalExpression        ( ( == | != ) RelationalExpression )*
 *     RelationalExpression     :   ShiftExpression             ( ( < | > | <= | >= ) ShiftExpression )*
 *     ShiftExpression          :   AdditiveExpression          ( ( << | >> ) AdditiveExpression )*
 *     AdditiveExpression       :   MultiplicativeExpression    ( ( + | - ) MultiplicativeExpression )*
 *     MultiplicativeExpression :   UnaryExpression             ( ( * | / | % ) UnaryExpression )*
 *     UnaryExpression          :   ( ++ | -- | + | - | ~ | ! | (TypeName) )* PostFixExpression
 *     PostFixExpression        :   PrimaryExpression
 *     PrimaryExpression        :   Value | Data | ( Expression )
 */
#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

#include "simplelexer.h"
#include "simplesymboltable.h"
#include "simpleast.h"

class Msg;

class SimpleParser
{
public:
    SimpleParser(SimpleLexer *lexer);
    SimpleNode *parse();

    SymbolTable &getSymblTbl();

private:
    void eat(SimpleToken::TokenType tokenType);
    SimpleNode *Expression();
    SimpleNode *ConditionalExpression();
    SimpleNode *LogicalORExpression();
    SimpleNode *LogicalXORExpression();
    SimpleNode *LogicalANDExpression();
    SimpleNode *BitwiseORExpression();
    SimpleNode *BitwiseXORExpression();
    SimpleNode *BitwiseANDExpression();
    SimpleNode *EqualityExpression();
    SimpleNode *RelationalExpression();
    SimpleNode *ShiftExpression();
    SimpleNode *AdditiveExpression();
    SimpleNode *MultiplicativeExpression();
    SimpleNode *UnaryExpression();
    SimpleNode *PostFixExpression();
    SimpleNode *PrimaryExpression();

    void SyntacticError(SharedSimpleTokenPtr Token);
    void TypeError(SharedSimpleTokenPtr Token);
    void EOFUnexpectedError(SharedSimpleTokenPtr Token);
    void EOFExpectedError(SharedSimpleTokenPtr Token);

    SimpleLexer *lexer;
    SharedSimpleTokenPtr CurrentToken;
    SymbolTable SymblTbl;
    bool ErrorOccured;
};

#endif // SIMPLEPARSER_H
