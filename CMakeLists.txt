project(notes NONE)
cmake_minimum_required(VERSION 2.4)

include(UseLATEX.cmake)
set(PDFLATEX_COMPILER pdflatex)

execute_process(COMMAND ${CMAKE_COMMAND}
  -E create_symlink ${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_BINARY_DIR}/src)

add_latex_document(notes.tex
  INPUTS basic-ds.tex concurrent.tex network.tex
         python.tex advanced-ds.tex cl.tex
  DEFAULT_PDF)
