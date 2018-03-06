<?php

function allow_eval($output) {
  eval("echo '{$output}';");
}

function another_eval($output) {
  eval("echo '{$output}';");
}

function test_it() {
  allow_eval("This text is rendered from the \'allow_eval\' function.");
  allow_eval("This text is rendered from the \'allow_eval\' function again.");
  another_eval("This text is rendered from the \'another_eval\' function.");
}

test_it();
