ls > out
cat out1
ls>>out3
cat < out3
ls | grep o
ls .. | tr a-z A-Z | grep A | head
ls | grep s >> out4
cat < out | tr a-z A-Z | tee out5 
cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2
