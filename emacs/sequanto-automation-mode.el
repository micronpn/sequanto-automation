(require 'generic-x)

(define-generic-mode 
  'sequanto-automation-mode
  '("#")                    
  '("name" "import" "function" "property"
    "monitor" "branch" "typedef" "enum")                     ;; some keywords
  nil
  '("\\.automation$")
  nil ;; other functions to call
  "Mode for editing Sequanto automation files."
)
