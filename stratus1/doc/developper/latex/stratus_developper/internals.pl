# LaTeX2HTML 2012 (1.2)
# Associate internals original text with physical files.


$key = q/secmodel/;
$ref_files{$key} = "$dir".q|node1.html|; 
$noresave{$key} = "$nosave";

$key = q/secnet/;
$ref_files{$key} = "$dir".q|node7.html|; 
$noresave{$key} = "$nosave";

$key = q/secinst/;
$ref_files{$key} = "$dir".q|node13.html|; 
$noresave{$key} = "$nosave";

1;

