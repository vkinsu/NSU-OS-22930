Like in lab 30, write in console:
```
  bash ./start.sh
```

How to check work on clients > 1?
First of all, on main mashine, who built the server, you need to write:
```
  chmod 777 qwerty12345656
```
qwerty12345656 - name of created server (like so)

After that in other mashines we need to start client work:
```
  ./client
```
All output will be on main stdout of main mashine, who built the server.
