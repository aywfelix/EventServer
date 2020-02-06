#!/bin/bash
kill -9 `ps aux | grep 'MasterServer' | awk -F' ' '{print $2}'`
kill -9 `ps aux | grep 'LoginServer' | awk -F' ' '{print $2}'`
kill -9 `ps aux | grep 'ChatServer' | awk -F' ' '{print $2}'`
kill -9 `ps aux | grep 'GateServer' | awk -F' ' '{print $2}'`
kill -9 `ps aux | grep 'GameServer' | awk -F' ' '{print $2}'`
kill -9 `ps aux | grep 'WorldServer' | awk -F' ' '{print $2}'`