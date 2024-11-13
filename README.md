# MP3 Tag Reader & Editor

***Project Overview:

The MP3 Tag Reader & Editor is a software tool designed to read and modify MP3 metadata, specifically ID3 tags, which store information about the audio file. This tool supports both ID3v1 and ID3v2 tag formats, allowing users to view and edit metadata such as the song title, artist, album, and year.

**About ID3 Tags:
ID3 tags are metadata containers in MP3 files that store information like title, artist, album, and track number. There are two main versions:

*ID3v1: This version occupies 128 bytes at the end of the MP3 file and includes fields like title, artist, album, and year.
*ID3v2: A more versatile version, ID3v2 tags are placed at the beginning of the MP3 file and allow for more detailed metadata with customizable frame headers.

**ID3 Tag Versions Supported:
->ID3v1:
*ID3v1.0
*ID3v1.1

->ID3v2:
*ID3v2.2
*ID3v2.3
*ID3v2.4

**Project Features:
1. View (Read) MP3 Tag Information
Displays MP3 metadata fields like title, artist, album, year, genre, etc., by reading the ID3 tag directly from the file.
2. Edit MP3 Tag Information

**Allows modification of ID3 tag information, updating fields such as:
*Title
*Artist
*Album
*Year
*Content Type
*Composer
3. Error Handling
The software includes error handling for incorrect file formats, missing metadata, and file access issues.

**Technical Requirements:
Programming Language: C (or C++ for advanced bit manipulation)
Libraries: Standard libraries for file I/O and memory handling
Supported File Formats: .mp3 files with ID3v1 or ID3v2 tags

**View Tags: ./mp3tag_reader --view <filename>.mp3

**Edit Tags: ./mp3tag_editor --edit <filename>.mp3 --field <field_name> --value <new_value>

**help : -h as command.