# Create the credentials file for the user to populate
touch AUTH_TOKEN

# Clone dependencies in to the include directory
git clone https://github.com/Tencent/rapidjson.git
mv rapidjson/include/rapidjson include/
rm -rf rapidjson
