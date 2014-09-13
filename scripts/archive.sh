#!/bin/bash

# Generate an archive from a list of objects
#
# Copyright (c) 2014, Brian McKenzie <mckenzba@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#
#  3. Neither the name of the organization nor the names of its contributors may
#     be used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

create() {
    local arprog=${1}
    local objlist=${2}
    local outlib=${3}
    local first=1

    if [ -z ${1} ]; then
        echo -e "Usage:\n\t${0} [ar program] [/path/to/objlist].list [outputfile].a"
    	return 1
    fi

    echo -n "Generating `basename ${outlib}`... "

    if [ -f ${outlib} ]; then
        rm -f ${outlib}
    fi

    for obj in `cat ${objlist}`; do
        if [ ${first} -ne 0 ]; then
            ${arprog} cr ${outlib} ${obj}
            first=0
            continue
	   fi

        ${arprog} r ${outlib} ${obj}
    done

    echo "Done."

    return 0
}

create ${@}

exit ${?}

