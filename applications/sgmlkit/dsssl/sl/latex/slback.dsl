<!doctype programcode public "-//Starlink//DTD DSSSL Source Code 0.2//EN" [
  <!entity common.dsl		system "../common/slcommon.dsl" subdoc>
  <!entity lib.dsl		system "../lib/sllib.dsl" subdoc>
]>
<!-- $Id$ -->

<docblock>
<title>Backmatter in LaTeX
<description>Support the backmatter elements.  Reasonably simple in
LaTeX, because there's little cross-referencing to do.  The only
wrinkle is that we want to produce the printed documentation in a
single LaTeX pass, so we need to generate the bibliography `by hand'
in a separate Jade pass.

<authorlist>
<author id=ng affiliation='Glasgow'>Norman Gray

<copyright>Copyright 1999, Particle Physics and Astronomy Research Council

<codegroup id=code.back>
<title>Support backmatter

<misccode>
<description>Declare Jade extension
<codebody>
(define read-entity
  (external-procedure "UNREGISTERED::James Clark//Procedure::read-entity"))
(declare-flow-object-class fi
  "UNREGISTERED::James Clark//Flow Object Class::formatting-instruction")

<misccode>
<description>Support backmatter elements
<codebody>

(mode section-reference
  (element backmatter
    (make-section-reference title: (literal "Notes, etc..."))))

(element note
  (make command name: "footnote"
	(process-children)))
;; do nothing with the notecontents element -- the NOTE element produces
;; footnotes rather than endnotes
(element notecontents
  (empty-sosofo))

<misccode>
<description>
Bibliography support.  The citation can be handled very simply -- just
emit the citation key, bracketed.  For the bibliography itself, simply
read in the <code/.bbl/ file we hope has been generated by a previous
Jade pass.
<codebody>
(element citation
  (make sequence
    (literal "[")
    (process-children)
    (literal "]")))

(element bibliography
  (let ((bibcontents (read-entity (string-append (root-file-name)
						 ".latexbib.bbl"))))
    (if bibcontents
	(make sequence
	  ($latex-section$ "section"
			   children: #f	;no children to process
			   )
	  (make fi data: bibcontents))
	(literal "No bibliography found"))))

(mode section-reference
  (element bibliography
    (make-section-reference title: (literal "Bibliography"))))


<codereference doc="lib.dsl" id="code.lib">
<title>Library code
<description>
<p>Some of this library code is from the standard, some from Norm
Walsh's stylesheet, other parts from me

<codereference doc="common.dsl" id="code.common">
<title>Common code
<description>
<p>Code which is common to both the HTML and print stylesheets.

<codegroup id=back.main use="code.common code.lib">
<title>Preprocess backmatter
<description>This part of the stylesheet is standalone, and may be used
to process a document and extract those parts of the document (such as 
bibliography references) which require preprocessing.

<misccode>
<description>
Extract the bibliography to a LaTeX .aux file, ready for processing
by BibTeX.
<codebody>
(declare-flow-object-class entity
  "UNREGISTERED::James Clark//Flow Object Class::entity")
(declare-flow-object-class fi
  "UNREGISTERED::James Clark//Flow Object Class::formatting-instruction")
(define debug
  (external-procedure "UNREGISTERED::James Clark//Procedure::debug"))


(root
    (make sequence
      (make fi data: (string-append (root-file-name) ":"))
      (get-bibliography)))

(define (get-bibliography)
  (let* ((kids (select-by-class (descendants (document-element)) 'element))
	 (citations (select-elements kids (normalize "citation")))
	 (bibelement (select-elements kids (normalize "bibliography"))))
    (if (node-list-empty? citations)
	(empty-sosofo)
	(make entity system-id: (string-append (root-file-name)
					       ".latexbib.aux")
	      (make fi data: "\\relax
")
	      (process-node-list citations)
	      (if (node-list-empty? bibelement)
		  (error "Citations but no BIBLIOGRAPHY in document")
		  (process-node-list bibelement))))))

(element citation
  (make fi data: (string-append "\\citation{" (trim-data (current-node)) "}
")))

(element bibliography
  (let ((citeall (attribute-string "ALL" (current-node))))
  (make sequence
    (if citeall
	(make fi data: "\\citation{*}
")
	(empty-sosofo))
    (make fi data: (string-append "\\bibdata{"
				  (attribute-string "BIB" (current-node))
				  "}
"))
    (make fi data: "\\bibstyle{plainlatex}
"))))
