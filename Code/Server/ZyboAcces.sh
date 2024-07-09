#!/bin/bash

# Define variables
REMOTE_USER="petalinux"
REMOTE_HOST="10.0.0.2"
REMOTE_PROGRAM="./Exchange_Unit_Platform"

# Establish SSH connection and execute the program
ssh ${REMOTE_USER}@${REMOTE_HOST} -S 
"sudo cd run && ${REMOTE_PROGRAM}"