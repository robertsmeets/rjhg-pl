union ASTdata {
        int intValue;
	float floatValue;
	void* ptrValue;
    };


struct AST {
    char operand;
    union ASTdata data;
    void* two;
    void* three;
    void* four;
};
