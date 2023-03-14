#!/bin/bash

text="src.diff"
dir="./src"

rm -rf $dir
mkdir $dir
awk '/^diff/{f="./src/"substr($3,match($3,/[A-Za-z.]*(cpp|h)/))}{print>>f}' "$text"
rg --files-without-match "new file mode" "$dir" | xargs rm
rg --files "$dir" | xargs -I{} sed -ie '1,6d' {}
rg --files "$dir" | xargs -I{} sed -i 's/^+//g' {}
