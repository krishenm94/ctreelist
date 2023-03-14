#!/bin/bash

awk '/^diff/{f=substr($3,match($3,/[A-Za-z.]*(cpp|h)/))}{print>>f}' src.diff
