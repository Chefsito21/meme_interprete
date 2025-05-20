#ifndef AST_VISUALIZER_H
#define AST_VISUALIZER_H

#include "ast.h"
#include <string>
#include <vector>
#include <memory>

class ASTVisualizer {
public:
    void exportToDot(const std::vector<std::shared_ptr<ASTNode>>& ast, const std::string& filename);
};

#endif // AST_VISUALIZER_H
