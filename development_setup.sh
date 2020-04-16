# Create the credentials file for the user to populate
touch config.json

# Clone dependencies in to the include directory
git clone https://github.com/Tencent/rapidjson.git
mv rapidjson/include/rapidjson include/
rm -rf rapidjson

# Clone C Python Requests include
git clone https://github.com/whoshuu/cpr.git
mv cpr/include/cpr include/
rm -rf cpr
