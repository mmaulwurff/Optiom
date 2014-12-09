About
=====

Optiom is a *ini configuration/settings manager.

Implemented with Qt.

![Screenshot](http://habrastorage.org/files/7e0/73f/c5b/7e073fc5b5f6427aabf12dc3f5d78e77.PNG)

Usage
-----

Features:
- Reads all *ini files in current directory and its subdirectories.
- Changes in fields are immediately written to corresponding files, no saving required.
- File backup/restore feature.
- Understands one level of *ini settings groups.
- Optiom understands field types by beginning of field name:
    * "num" - numeric fields (from 0 to large integers), e.g. num_bytes, numberOfBytes
    * "per" - percent fields (from 0 to 100), e.g. percentHealth
    * "is_" - boolean fields (true/false), e.g. is_feature_on
    * other - text fields.

[Source code is available at GitHub](https://github.com/mmaulwurff/Optiom)

Copyright (C) 2014 Alexander Kromm (mmaulwurff@gmail.com).

Optiom is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

Optiom is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Optiom. If not, see <http://www.gnu.org/licenses/>.
