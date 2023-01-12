#include "include/log.hpp"
#include "include/common.hpp"
#include "include/compile.hpp"
#include "include/file.hpp"

#include "fe/lexer.hpp"
#include "fe/parser.hpp"

namespace rotate
{
void
log_compilation(FILE *output, file_t *code_file, Lexer *lexer, Parser *parser)
{
    time_t rawtime;
    time(&rawtime);
    assert(code_file && lexer);

    const auto *tokens = lexer->getTokens();
    if (tokens->size() > 0x10000000)
    {
        log_warn("Too large file to show log");
        return;
    }
    log_warn("Logging will slow down compiliation ");
    fprintf(output, "#+TITLE: COMPILATION LOG\n");
    fprintf(output, "#+OPTIONS: toc:nil num:nil\n");
    fprintf(output, "#+AUTHOR: Rotate compiler\n");
    fprintf(output, "#+DATE: %s\n", asctime(localtime(&rawtime)));
    fprintf(output, "** Meta\n");
    fprintf(output, "- filename: =%s=\n", code_file->name);
    fprintf(output, "- file length(chars): %u chars\n", code_file->length);
    fprintf(output, "- time: %s", asctime(localtime(&rawtime)));
    fprintf(output, "- number of tokens: %lu\n\n", tokens->size());
    fprintf(output, "** FILE\n");
    fprintf(output, "#+begin_src cpp \n%s\n#+end_src\n\n", code_file->contents);

    // TOKENS LOG STAGE
    fprintf(output, "** TOKENS\n");
    fprintf(output, "#+begin_src\n");
    for (UINT i = 0; i < tokens->size(); i++)
    {
        const Token &tkn = tokens->at(i);
        fprintf(output, "[TOKEN]: n: %u, idx: %u, line: %u, len: %u, type: %s, val: `%.*s`\n", i,
                tkn.index, tkn.line, tkn.length, tkn_type_describe(tkn.type), tkn.length,
                code_file->contents + tkn.index);
    }
    fprintf(output, "#+end_src\n");

    // PARSER STAGE
    fprintf(output, "\n** Parser Abstract Syntax Tree\n");
    fprintf(output, "*** Imports \n");
    fprintf(output, "#+begin_src\n");
    for (UINT i = 0; i < parser->ast->imports.size(); i++)
    {
        const AstImport &m = parser->ast->imports[i];
        if (m.aliased)
            fprintf(output, "[IMPORT]: n: %u, import_str_idx: %u, alias_idx: %u\n", i, m.import_str,
                    m.alias_id);
        else
            fprintf(output, "[IMPORT]: n: %u, id_idx: %u \n", i, m.import_str);
    }
    fprintf(output, "#+end_src\n");
    fprintf(output, "\n** TODO TYPECHECKER\n");
    log_info("Logging complete");
}

} // namespace rotate
