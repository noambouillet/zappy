#!/bin/bash

# Test script for zappy_server argument parsing
# Usage: ./test_parsing.sh [path/to/zappy_server]

SERVER="${1:-./zappy_server}"
PASS=0
FAIL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

find_free_port() {
    python3 -c "
import socket
s = socket.socket()
s.bind(('', 0))
print(s.getsockname()[1])
s.close()
" 2>/dev/null || echo 59876
}
FREE_PORT=$(find_free_port)

if [ ! -x "$SERVER" ]; then
    echo -e "${YELLOW}zappy_server doesn't exist or isn't executable.${NC}"
    exit 1
fi

run_test() {
    local description="$1"
    local expected_exit="$2"
    local expected_pattern="$3"
    shift 3
    local args=("$@")

    output=$(timeout 2s "$SERVER" "${args[@]}" 2>&1)
    actual_exit=$?
    [ "$actual_exit" -eq 124 ] && actual_exit=0

    local effective_exit="$actual_exit"
    if [ "$expected_exit" -eq 0 ] && [ "$actual_exit" -eq 84 ]; then
        echo "$output" | grep -q "\-\-\-\-\-\-" && effective_exit=0
    fi

    local exit_ok=false
    if [ "$expected_exit" = "any" ]; then
        exit_ok=true
    elif [ "$effective_exit" -eq "$expected_exit" ]; then
        exit_ok=true
    fi

    local pattern_ok=true
    if [ -n "$expected_pattern" ]; then
        echo "$output" | grep -qE "$expected_pattern" || pattern_ok=false
    fi

    if $exit_ok && $pattern_ok; then
        echo -e "${GREEN}[PASS]${NC} $description"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $description"
        echo -e "       Args    : ${args[*]}"
        if ! $exit_ok; then
            echo -e "       Exit    : expected=$expected_exit  got=$actual_exit (effective=$effective_exit)"
        fi
        if ! $pattern_ok; then
            echo -e "       Pattern : '$expected_pattern' not found in output"
        fi
        echo -e "       Output  : $(echo "$output" | tail -3)"
        FAIL=$((FAIL + 1))
    fi
}

echo -e "${YELLOW}========================================${NC}"
echo -e "${YELLOW}  zappy_server – Argument Parsing Tests ${NC}"
echo -e "${YELLOW}========================================${NC}"
echo -e "${CYAN}Using free port $FREE_PORT for valid-call tests${NC}"

echo -e "\n${YELLOW}--- Help & no-args ---${NC}"
run_test "No arguments → usage + error"          84  "USAGE"
run_test "--help → usage + exit 0"                0  "USAGE"           --help
run_test "-h → unknown flag"                     84  "Unknown"         -h

echo -e "\n${YELLOW}--- Minimal valid calls ---${NC}"
run_test "Only -n (one team)"                     0  "Teams"           -n team1 -p $FREE_PORT
run_test "Only -n (multiple teams)"               0  "Teams"           -n team1 team2 -p $FREE_PORT
run_test "-n + all flags"                         0  "Teams"           -n team1 team2 -p $FREE_PORT -x 20 -y 20 -c 5 -f 50

echo -e "\n${YELLOW}--- Port (-p) ---${NC}"
run_test "-p lower bound (1024)"                  0  "port = 1024"     -n team1 -p 1024
run_test "-p upper bound (65535)"                 0  "port = 65535"    -n team1 -p 65535
run_test "-p in range"                            0  "port = $FREE_PORT" -n team1 -p $FREE_PORT
run_test "-p below lower bound (1023)"           84  "Invalid port"    -n team1 -p 1023
run_test "-p above upper bound (65536)"          84  "Invalid port"    -n team1 -p 65536
run_test "-p zero"                               84  "Invalid port"    -n team1 -p 0
run_test "-p negative"                           84  "Invalid port"    -n team1 -p -1
run_test "-p non-numeric"                        84  "Invalid port"    -n team1 -p abc
run_test "-p float"                              84  "Invalid port"    -n team1 -p 1.5
run_test "-p empty string"                       84  ""                -n team1 -p ""
run_test "-p missing value (end of args)"        84  ""                -n team1 -p

echo -e "\n${YELLOW}--- Width (-x) ---${NC}"
run_test "-x lower bound (10)"                    0  "width = 10"      -n team1 -p $FREE_PORT -x 10
run_test "-x upper bound (42)"                    0  "width = 42"      -n team1 -p $FREE_PORT -x 42
run_test "-x mid value (25)"                      0  "width = 25"      -n team1 -p $FREE_PORT -x 25
run_test "-x below lower bound (9)"              84  "Invalid width"   -n team1 -p $FREE_PORT -x 9
run_test "-x above upper bound (43)"             84  "Invalid width"   -n team1 -p $FREE_PORT -x 43
run_test "-x zero"                               84  "Invalid width"   -n team1 -p $FREE_PORT -x 0
run_test "-x negative"                           84  "Invalid width"   -n team1 -p $FREE_PORT -x -5
run_test "-x non-numeric"                        84  "Invalid width"   -n team1 -p $FREE_PORT -x xyz
run_test "-x missing value (end of args)"        84  ""                -n team1 -p $FREE_PORT -x

echo -e "\n${YELLOW}--- Height (-y) ---${NC}"
run_test "-y lower bound (10)"                    0  "height = 10"     -n team1 -p $FREE_PORT -y 10
run_test "-y upper bound (42)"                    0  "height = 42"     -n team1 -p $FREE_PORT -y 42
run_test "-y mid value (30)"                      0  "height = 30"     -n team1 -p $FREE_PORT -y 30
run_test "-y below lower bound (9)"              84  "Invalid height"  -n team1 -p $FREE_PORT -y 9
run_test "-y above upper bound (43)"             84  "Invalid height"  -n team1 -p $FREE_PORT -y 43
run_test "-y zero"                               84  "Invalid height"  -n team1 -p $FREE_PORT -y 0
run_test "-y negative"                           84  "Invalid height"  -n team1 -p $FREE_PORT -y -1
run_test "-y non-numeric"                        84  "Invalid height"  -n team1 -p $FREE_PORT -y !!
run_test "-y missing value (end of args)"        84  ""                -n team1 -p $FREE_PORT -y

echo -e "\n${YELLOW}--- Clients (-c) ---${NC}"
run_test "-c lower bound (1)"                     0  "clients_nb = 1"  -n team1 -p $FREE_PORT -c 1
run_test "-c upper bound (200)"                   0  "clients_nb = 200" -n team1 -p $FREE_PORT -c 200
run_test "-c mid value (10)"                      0  "clients_nb = 10" -n team1 -p $FREE_PORT -c 10
run_test "-c below lower bound (0)"              84  "Invalid clients" -n team1 -p $FREE_PORT -c 0
run_test "-c above upper bound (201)"            84  "Invalid clients" -n team1 -p $FREE_PORT -c 201
run_test "-c negative"                           84  "Invalid clients" -n team1 -p $FREE_PORT -c -1
run_test "-c non-numeric"                        84  "Invalid clients" -n team1 -p $FREE_PORT -c foo
run_test "-c missing value (end of args)"        84  ""                -n team1 -p $FREE_PORT -c

echo -e "\n${YELLOW}--- Frequency (-f) ---${NC}"
run_test "-f lower bound (1)"                     0  "freq = 1"        -n team1 -p $FREE_PORT -f 1
run_test "-f upper bound (10000)"                 0  "freq = 10000"    -n team1 -p $FREE_PORT -f 10000
run_test "-f mid value (100)"                     0  "freq = 100"      -n team1 -p $FREE_PORT -f 100
run_test "-f below lower bound (0)"              84  "Invalid freq"    -n team1 -p $FREE_PORT -f 0
run_test "-f above upper bound (10001)"          84  "Invalid freq"    -n team1 -p $FREE_PORT -f 10001
run_test "-f negative"                           84  "Invalid freq"    -n team1 -p $FREE_PORT -f -1
run_test "-f non-numeric"                        84  "Invalid freq"    -n team1 -p $FREE_PORT -f nope
run_test "-f missing value (end of args)"        84  ""                -n team1 -p $FREE_PORT -f

echo -e "\n${YELLOW}--- Team names (-n) ---${NC}"
run_test "-n GRAPHIC reserved name"              84  "reserved"        -n GRAPHIC -p $FREE_PORT
run_test "-n duplicate team names"               84  "Duplicate"       -n alpha alpha -p $FREE_PORT
run_test "-n many teams (6)"                      0  "Teams"           -n a b c d e f -p $FREE_PORT
run_test "-n teams with hyphens/underscores"      0  "Teams"           -n my-team my_team -p $FREE_PORT
run_test "-n after other flags"                   0  "Teams"           -p $FREE_PORT -n team1
run_test "-n missing names (no values)"          84  ""                -n
run_test "Missing -n entirely"                   84  "Team names"      -p $FREE_PORT -x 10 -y 10 -c 1 -f 1

echo -e "\n${YELLOW}--- Argument order & combinations ---${NC}"
run_test "All flags before -n"                    0  "Teams"           -p $FREE_PORT -x 15 -y 15 -c 2 -f 10 -n alpha beta
run_test "-n sandwiched between flags"            0  "Teams"           -p $FREE_PORT -n team1 -x 15 -y 15
run_test "Repeated -p (last wins)"                0  "port = $FREE_PORT" -p 1111 -p $FREE_PORT -n team1
run_test "Repeated -x (last wins)"                0  "width = 20"      -n team1 -p $FREE_PORT -x 10 -x 20

echo -e "\n${YELLOW}--- Unknown / malformed options ---${NC}"
run_test "Unknown flag -z (before -n)"           84  "Unknown"         -z -n team1 -p $FREE_PORT
run_test "Unknown flag --port (before -n)"       84  "Unknown"         --port 5555 -n team1 -p $FREE_PORT
run_test "Unknown flag -z (after other flags)"   84  "Unknown"         -n team1 -p $FREE_PORT -z
run_test "--port after -p (consumed as team)"     0  "Teams"           -n team1 --port 5555 -p $FREE_PORT

TOTAL=$((PASS + FAIL))
echo -e "\n${YELLOW}========================================${NC}"
echo -e "Results: ${GREEN}$PASS passed${NC} / ${RED}$FAIL failed${NC} / $TOTAL total"
echo -e "${YELLOW}========================================${NC}"

[ "$FAIL" -eq 0 ] && exit 0 || exit 1
