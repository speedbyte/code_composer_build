
..\12_tools\srec_cat ^
%1.plf ^
-Binary ^
-Output parrot_1.hex ^
-Intel

..\12_tools\srec_cat ^
parrot_1.hex ^
-Intel ^
-Output %1.hex ^
-Intel

del parrot_1.hex