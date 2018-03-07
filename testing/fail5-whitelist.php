<?php

function allow_eval($output) {
  eval("echo '{$output}';");
}

function another_eval($output) {
  eval("echo '{$output}';");
}

function allow_wrapper() {
  allow_eval("This text is rendered from the \'allow_eval\' function, called from \'allow_wrapper\'.\n");
}

function test_it() {
  allow_eval("This text is rendered from the \'allow_eval\' function.\n");
  allow_eval("This text is rendered from the \'allow_eval\' function again.\n");
  allow_wrapper();
  another_eval("This text is rendered from the \'another_eval\' function.\n");
}

allow_eval("This text is rendered from the \'allow_eval\' function.\n");
allow_eval("This text is rendered from the \'allow_eval\' function again.\n");
allow_wrapper();
test_it();
