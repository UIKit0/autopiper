/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _AUTOPIPER_FRONTEND_AST_H_
#define _AUTOPIPER_FRONTEND_AST_H_

#include <vector>
#include <memory>
#include <string>

#include "common/parser-utils.h"

namespace autopiper {
namespace frontend {

template<typename T>
using ASTVector = std::vector<std::unique_ptr<T>>;

template<typename T>
using ASTRef = std::unique_ptr<T>;

typedef Token::bignum ASTBignum;

struct AST;
struct ASTFunctionDef;
struct ASTTypeDef;

struct ASTIdent;
struct ASTType;
struct ASTParam;

struct ASTStmt;
struct ASTStmtBlock;
struct ASTStmtLet;
struct ASTStmtAssign;
struct ASTStmtIf;
struct ASTStmtWhile;
struct ASTStmtBreak;
struct ASTStmtContinue;
struct ASTStmtWrite;
struct ASTStmtSpawn;

struct ASTExpr;

struct ASTTypeField;
struct ASTTypeFieldRef;

struct AST {
    ASTVector<ASTFunctionDef> functions;
    ASTVector<ASTTypeDef> types;
};

struct ASTFunctionDef {
    ASTRef<ASTIdent> name;
    ASTRef<ASTType> return_type;
    ASTVector<ASTParam> params;
    ASTRef<ASTStmtBlock> block;
    bool is_entry;

    ASTFunctionDef() : is_entry(false)  {}
};

struct ASTParam {
    ASTRef<ASTIdent> ident;
    ASTRef<ASTType> type;
};

struct ASTTypeDef {
    ASTRef<ASTIdent> ident;
    ASTVector<ASTTypeField> fields;
};

struct ASTTypeField {
    ASTRef<ASTIdent> ident;
    ASTRef<ASTType> type;
};

struct ASTIdent {
    std::string name;

    enum Type {
        FUNC,
        VAR,
        TYPE,
    };
    Type type;
};

struct ASTType {
    ASTRef<ASTIdent> ident;
    bool is_port;

    ASTTypeDef* def;

    ASTType() : is_port(false), def(nullptr)  {}
};

struct ASTStmt {
    ASTRef<ASTStmtBlock> block;
    ASTRef<ASTStmtLet> let;
    ASTRef<ASTStmtAssign> assign;
    ASTRef<ASTStmtIf> if_;
    ASTRef<ASTStmtWhile> while_;
    ASTRef<ASTStmtBreak> break_;
    ASTRef<ASTStmtContinue> continue_;
    ASTRef<ASTStmtWrite> write;
    ASTRef<ASTStmtSpawn> spawn;
};

struct ASTStmtBlock {
    ASTVector<ASTStmt> stmts;
};

struct ASTStmtLet {
    ASTRef<ASTIdent> lhs;
    ASTRef<ASTType> type;
    ASTRef<ASTExpr> rhs;
};

struct ASTStmtAssign {
    // TODO: support array and type-field refs on LHS.
    ASTRef<ASTIdent> lhs;
    ASTRef<ASTExpr> rhs;
};

struct ASTStmtIf {
    ASTRef<ASTExpr> condition;
    ASTRef<ASTStmt> if_body;
    ASTRef<ASTStmt> else_body;
};

struct ASTStmtWhile {
    ASTRef<ASTExpr> condition;
    ASTRef<ASTStmt> body;
};

struct ASTStmtBreak {
    ASTStmtWhile* enclosing_while;  // filled in later, after parse

    ASTStmtBreak() : enclosing_while(nullptr)  {}
};

struct ASTStmtContinue {
    ASTStmtWhile* enclosing_while;  // filled in later, after parse

    ASTStmtContinue() : enclosing_while(nullptr)  {}
};

struct ASTStmtWrite {
    ASTRef<ASTIdent> port;
    ASTRef<ASTExpr> rhs;
};

struct ASTStmtSpawn {
    ASTRef<ASTStmt> body;
};

struct ASTExpr {
    enum Op {
        ADD,
        SUB,
        MUL,
        DIV,
        REM,
        AND,
        OR,
        NOT,
        LSH,
        RSH,
        BITSLICE,
        CONCAT,

        VAR,
        CONST,

        PORTREAD,

        PORTDEF,
    };

    Op op;

    ASTVector<ASTExpr> ops;
    ASTRef<ASTIdent> ident;
    ASTBignum constant;

    ASTRef<ASTType> type;  // inferred; not from parser
};

}  // namespace frontend
}  // namespace autopiper

#endif  // _AUTOPIPER_FRONTEND_AST_H_