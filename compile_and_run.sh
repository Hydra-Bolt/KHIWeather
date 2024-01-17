
# Load environment variables from .env file
if [ -f .env ]; then
  source .env
else
  echo "Error 404: env not found"
  exit 1
fi

# Function to check if a variable is a positive integer
is_positive_integer() {
  local value=$1
  if [[ $value =~ ^[1-9][0-9]*$ ]]; then
    return 0  # Valid positive integer
  else
    return 1  # Not a valid positive integer
  fi
}

# Function to check if a variable is a valid parameter (1 to 5)
is_valid_parameter() {
  local value=$1
  if [ "$value" -ge 1 ] && [ "$value" -le 5 ]; then
    return 0  # Valid parameter
  else
    return 1  # Not a valid parameter
  fi
}
is_valid_positive_integer() {
  local value=$1
  local lower_limit=$2
  local upper_limit=$3
  if [ "$value" -gt "$lower_limit" ] && [ "$value" -lt "$upper_limit" ]; then
    return 0  # Valid positive integer within the range
  else
    return 1  # Not a valid positive integer within the range
  fi
}

# Checking validity of each argument
if ! is_valid_positive_integer "$CITY" 0 9; then
  echo "Error: CITY must be a positive integer between 1 and 8."
  exit 1
fi

if ! is_valid_positive_integer "$NUM_OF_DAYS" 0 17; then
  echo "Error: NUM_OF_DAYS must be a positive integer between 1 and 16."
  exit 1
fi

if ! is_valid_parameter "$PARAM_1"; then
  echo "Error: PARAM_1 must be a valid parameter (1 to 5)."
  exit 1
fi

if ! is_valid_parameter "$PARAM_2"; then
  echo "Error: PARAM_2 must be a valid parameter (1 to 5)."
  exit 1
fi

if ! is_valid_parameter "$PARAM_3"; then
  echo "Error: PARAM_3 must be a valid parameter (1 to 5)."
  exit 1
fi
gcc main.c apiparams.c analysis.c plotting.c emailanomalies.c processdata.c weathercast.c -o main -lcurl -lcjson -lplplot -lncurses -lhpdf -Wall 
./main $CITY 2 $NUM_OF_DAYS $PARAM_1 $PARAM_2 $PARAM_3 $EMAIL