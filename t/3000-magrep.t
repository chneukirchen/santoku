#!/bin/sh -e
cd ${0%/*}
. ./lib.sh
plan 10

rm -rf test.dir
mkdir test.dir
(
cd test.dir

mkdir -p "inbox/cur"

cat <<! | mmime >"inbox/cur/1:2,"
From: Piet Pompies <piet@lpompies.com>
Subject: wow nice subject
Date: Thu, 30 Mar 2017 15:42:05 +0200
Message-Id: <EOH1F3NUOY.2KBVMHSBFATNY@example.org>

shit happens
!

cat <<! | mmime >"inbox/cur/2:2,"
From: Piet Pompies <piet@pompies.com>
Subject: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
Date: Thu, 30 Mar 2017 15:42:05 +0200
Message-Id: <EOH1F3NUOY.2KBVMHSBFATNY@example.org>

Greetings
!

cat <<! | mmime >"inbox/cur/3:2,"
From: Piet Pompies <piet@pompies.com>
Subject: 1 multi subject one
Subject: 2 multi subject two
Subject: 3 multi subject three
Date: Thu, 30 Mar 2017 15:42:05 +0200
Message-Id: <EOH1F3NUOY.2KBVMHSBFATNY@example.org>

!

cat <<! | mmime >"inbox/cur/4:2,"
To: "John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>,
	"John Doe" <example@example.com>
!

cat <<! >seq
inbox/cur/1:2,
inbox/cur/2:2,
inbox/cur/3:2,
inbox/cur/4:2,
!

export MAILSEQ=seq

check_test 'subject' -eq 1 'magrep subject:nice : | wc -l'
check_test 'ignorecase' -eq 1 'magrep -i subject:NICE : | wc -l'
check_test 'invert' -eq 2 'magrep -v subject:nice : | wc -l'
check_test 'invert missing' -eq 1 'magrep -v subject:i : | wc -l'
check_test 'max matches' -eq 2 'magrep -m 2 from:Piet : | wc -l'
check_test 'long subject' -eq 1 'magrep subject:aliqua : | wc -l'
check_test 'decode large rfc2047 header' -eq 1 'magrep -d to:John : | wc -l'

echo 'inbox/cur/1:2,: subject: wow nice subject' >expect
check_same 'print' 'magrep -p subject:nice :' 'cat expect'

echo 'inbox/cur/1:2,: subject: nice' >expect
check_same 'print match' 'magrep -po subject:nice :' 'cat expect'

echo 'nice' >expect
check_same 'print match only' 'magrep -o subject:nice :' 'cat expect'

echo 'inbox/cur/3:2,' >expect
check_same 'multiple subjects' 'magrep subject:multi :' 'cat expect'

)
