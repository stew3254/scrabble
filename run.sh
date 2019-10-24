#!/bin/bash

make $1 1>/dev/null && bin/$1 ${@:2}
