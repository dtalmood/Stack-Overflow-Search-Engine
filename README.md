# Stack-Overflow-Search-Engine
Have you ever felt that the stackoverflow search results do not match what you are looking for? 

This app was made inorder to make searching for question you want to see easier. 

This application has full User Authetnication via MongoDB and once you create your account you can use the search engine. 

Features

**Sort:** Provides user options sort the results by either. 

  1. Views
  2. Answer Count
  3. Score

**Tags:** Allows users to have speicifc tags to ensure results relate more closely to what you are looking for. 

**Filters:** Any infomration you do or do not care to see can be simply adjusted via the filters which can remove the following from search result 
  1. Acceptance Rate
  2. Answer Count
  3. Creation Date
  4. Reputation
  5. Score
  6. Views

**How to start Mongo DB**

Step 1: Run command below to start Mongo DB server with the configurations which are already setup in mongod.conf.


```mongod --config /opt/homebrew/etc/mongod.conf```

Step 2:  To ensure that mongoDB server is running,  run command below( you should see mongoDB has community started).

```brew services list```


Step 3: Open a new terminal and run command below. This will connect us to the server. It states we want to connect to mongoDB servers running on local host and listening for port 27017.

```mongosh --host localhost --port 27017 ```

**Now if you open **


Step 4: Once you are ready to disconect to the server (mongosh instnace which connects to local host 27017) type 

```exit```

Step 9: and to stop the server, run command below 

```brew services stop mongodb/brew/mongodb-community```



**Express Instalation:** If you already have Xcode, and Homebrew installed you can skip to Step 4.

Step 1: Install xcode command line tools (this allows for installing necesary libraries).

```xcode-select --install ```

Step 2: Donwload Homebrew.

``` /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" ```


Step 3: After last command completes, copy the two commands in section labled **next step** below is example of what the commands will look like.

```Example 1: (echo; echo 'eval "$(/opt/homebrew/bin/brew shellenv)"') >> /Users/YOUR_USER_PROFILE/.zprofile```

```Example 2: eval "$(/opt/homebrew/bin/brew shellenv)"```


Step 4: After words Download the File "express.sh" (be sure your in directly where this file is located after donwloading) and run 2 commands below inorder.

``` chmod +x express.sh```


```./express.sh``` 










  




