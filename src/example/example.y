	

    program : expr_list { $$ = $1; $$->eval(new Table()); } ;
     
    multi_lf : /* empty */ | multi_lf LF ;
     
    expr_list : multi_lf expr_list2 multi_lf { $$ = $1; } ;
     
    expr_list2 : expr_list2 LF multi_lf expr { $$ = new BlockNode($1, $4); }
               | expr                     { $$ = $1; }
               ;
     
    expr : lvalue                                { $$ = new MemberNode($1.node, $1.name); }
         | lvalue ASSIGN expr                    { $$ = new AssignmentNode($1.node, $1.name, $3); }
         | OPAREN expr CPAREN                    { $$ = $2; }
         | expr OPAREN call_param_list CPAREN    { $$ = new CallNode($1, $3); }
         | FUNC func_param_list LF expr_list END { $$ = new FuncNode($2, $4); }
         
         | IF expr LF expr_list ENDIF                { $$ = new ConditionalNode($2, $4); }
         | IF expr LF expr_list ELSE expr_list ENDIF { $$ = new ConditionalNode($2, $4, $6); }
         | WHILE expr LF expr_list LOOP              { $$ = new LoopNode($2, $4, false); }
         
         | INTEGER                               { $$ = new IntegerNode(atol($1)); }
         ;
         
     
    lvalue : expr DOT NAME { $$.node = $1; $$.name = $3; }
           | NAME          { $$.node = NULL; $$.name = $1; }
           ;
     
    func_param_list : /* empty */                      { $$ = new NodeFuncParamList(); }
                    | func_param                       { $$ = new NodeFuncParamList(); if ($1.node) $$->add($1.name, $1.node); else $$->add($1.name); }
                    | func_param_list COMMA func_param { $$ = $1; if ($3.node) $$->add($3.name, $3.node); else $$->add($3.name); }
                    ;
     
    call_param_list : /* empty */                      { $$ = new NodeCallParamList(); }
                    | call_param                       { $$ = new NodeCallParamList(); if ($1.name) $$->addNamed($1.name, $1.node); else $$->addPositional($1.node); }
                    | call_param_list COMMA call_param { $$ = $1; if ($3.name) $$->addNamed($3.name, $3.node); else $$->addPositional($3.node); }
                    ;
     
    func_param : NAME            { $$.name = $1; $$.node = NULL; }
               | NAME COLON expr { $$.name = $1; $$.node = $3; }
               ;
     
    call_param : expr            { $$.name = NULL; $$.node = $1; }
               | NAME COLON expr { $$.name = $1; $$.node = $3; }
               ;


