@echo off
cd /d ..
call venv\Scripts\activate
cd /d UI
pyuic5 -x Core.ui -o ..\CoreUI.py
