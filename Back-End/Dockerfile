# Use the official Node.js image as the base image
FROM node:22

# Set the working directory to /app
WORKDIR /app

# Copy package.json and yarn.lock from the root directory to /app in the container
COPY package.json ./
COPY yarn.lock ./

# Clear npm cache and install dependencies
RUN yarn
#npm install

# Copy the entire application source code into the container
COPY . .

# Expose the port your app runs on
EXPOSE 8000

# Start the Express app
CMD ["yarn", "launch"] 
