UPDATE `command` SET `help` = 'Syntax: .idleshutdown #delay|cancel\r\n\r\nShut the server down after #delay seconds if no active connections are present (no players) or cancel the shutdown if cancel value is used.' WHERE `name` = 'idleshutdown'; 
UPDATE `command` SET `help` = 'Syntax: .shutdown #delay|cancel\r\n\r\nShut the server down after #delay seconds or cancel the shutdown if cancel value is used.' WHERE `name` = 'shutdown';
