FILE (GLOB HTML_FILES *.html)
FILE (GLOB CSS_FILES *.css)

INSTALL(FILES ${HTML_FILES} DESTINATION share/plexy/facebook/ui/data/)
INSTALL(FILES ${CSS_FILES} DESTINATION share/plexy/facebook/ui/data/)
