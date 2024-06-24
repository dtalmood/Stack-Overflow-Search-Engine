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

**Express Instalation:**

First, if not already install homebrew by either visiting the website or run command below in terminal
  
  ```/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" ```


After words Download the File "express.sh" and run the script ( this will install all necesary libaries to make instalation easy)

```./express.sh``` 


**Installation Manual:**

  if you would like to know what files are required Below is all Libraries used and how to isntall them one by one 

  Note: Ensure Homebrew is installed (home brew is a package manager for macOS that simplifies the installation of software)

  Install homebrew vist the website or run command below in terminal
  
  ```/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" ```

  Install Mongo DB run commands one after another
  
  ```brew tap mongodb/brew```
  
  ```brew install mongodb-community```

  Start Mongo DB
  
  ```brew services start mongodb/brew/mongodb-community```

  To ensure Mongo DB is Running:
  
  ```ps aux | grep -v grep | grep mongod```

  Connect to Local Host 27017
  
  ```mongo --host 127.0.0.1 --port 27017```

  Install Bcrypt 

  ```brew install bcrypt ```

 install Mongo-Cxx Driver
 
  ```brew install mongocxx ```




  




