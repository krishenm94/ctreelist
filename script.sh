#!/bin/bash

text="src.diff"
dir="./src/"

awk '/^diff/{f=$dir substr($3,match($3,/[A-Za-z.]*(cpp|h)/))}{print>>f}' "$text"
rg --files-without-match "new file mode" "$dir" | xargs echo
rg --files-without-match "new file mode" "$dir" | xargs rm
#rg --files "$dir" | xargs -n2 -I{} tail -n +6 {} >temp_{}
