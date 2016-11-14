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
 *     Expresion:                   ConditionalExpresion
 *     ConditionalExpression:       LogicalORExpression         ( ? Expression : LogicalORExpression )*
 *     LogicalORExpression:         LogicalXORExpression        ( || LogicalXORExpression )*
 *     LogicalXORExpression:        LogicalANDExpression        ( ^^ LogicalANDExpression )*
 *     LogicalANDExpression:        BitwiseORExpression         ( && BitwiseORExpression )*
 *     BitwiseORExpression:         BitwiseXORExpression        ( | BitwiseXORExpression )*
 *     BitwiseXORExpression:        BitwiseANDExpression        ( ^ BitwiseANDExpression )*
 *     BitwiseANDExpression:        EqualityExpression          ( & EqualityExpression )*
 *     EqualityExpression:          RelationalExpression        ( ( == | != ) RelationalExpression )*
 *     RelationalExpression:        ShiftExpression             ( ( < | > | <= | >= ) ShiftExpression )*
 *     ShiftExpression:             AdditiveExpression          ( ( << | >> ) AdditiveExpression )*
 *     AdditiveExpression:          MultiplicativeExpression    ( ( + | - ) MultiplicativeExpression )*
 *     MultiplicativeExpression:    UnaryExpression             ( ( * | / | % ) UnaryExpression )*
 *     UnaryExpression:             ( ++ | -- | + | - | ~ | ! | (TypeName) )* PostFixExpression
 *     PostFixExpression:           PrimaryExpression
 *     PrimaryExpression:           Value | Data | ( Expression )
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
    ValueNode *parse();

    SymbolTable &getSymblTbl();

private:
    void eat(const SimpleToken &token, SimpleToken::TokenType tokenType);
    const ASTNode * const Expression();
    const ASTNode * const ConditionalExpression();
    const ASTNode * const LogicalORExpression();
    const ASTNode * const LogicalXORExpression();
    const ASTNode * const LogicalANDExpression();
    const ASTNode * const BitwiseORExpression();
    const ASTNode * const BitwiseXORExpression();
    const ASTNode * const BitwiseANDExpression();
    const ASTNode * const EqualityExpression();
    const ASTNode * const RelationalExpression();
    const ASTNode * const ShiftExpression();
    const ASTNode * const AdditiveExpression();
    const ASTNode * const MultiplicativeExpression();
    const ASTNode * const UnaryExpression();
    const ASTNode * const PostFixExpression();
    const ASTNode * const PrimaryExpression();

    SimpleLexer *lexer;
    SimpleToken CurrentToken;
    SymbolTable SymblTbl;
};

#endif // SIMPLEPARSER_H
