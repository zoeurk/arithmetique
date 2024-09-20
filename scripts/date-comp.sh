#!/bin/sh
sed 'N; s/\(.*\)\n\(.*\)/\2-\1/' date.comp | bc
