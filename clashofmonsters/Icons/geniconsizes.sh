#!/bin/bash
sips --resampleWidth 512 $1 --out iTunesArtwork
sips --resampleWidth 1024 $1 --out iTunesArtwork@2x
sips --resampleWidth 57 $1 --out Icon.png
sips --resampleWidth 114 $1 --out Icon@2x.png
sips --resampleWidth 72 $1 --out Icon-72.png
sips --resampleWidth 144 $1 --out Icon-72@2x.png
sips --resampleWidth 29 $1 --out Icon-Small.png
sips --resampleWidth 58 $1 --out Icon-Small@2x.png
sips --resampleWidth 50 $1 --out Icon-Small-50.png
sips --resampleWidth 100 $1 --out Icon-Small-50@2x.png
