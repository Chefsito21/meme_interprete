#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ast.h"
#include <vector>
#include <memory>
#include <string>

class Optimizer {
public:
    void optimize(const std::vector<std::shared_ptr<ASTNode>>& ast, const std::string& outputPath);
};

#endif // OPTIMIZER_H
