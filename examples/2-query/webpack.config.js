const path = require("path");

module.exports = {
  entry: path.join(__dirname, "src/index.bs.js"),
  mode: "development",
  output: {
    path: path.resolve(__dirname, "build"),
    publicPath: "/public/",
    filename: "index.js"
  },
  devServer: {
    open: true,
    contentBase: path.resolve(__dirname, "public")
  },
  resolve: {
   alias: {
    react: path.resolve('./node_modules/react'),
     urql: path.resolve('./node_modules/urql'),
    "react-dom": path.resolve('./node_modules/react-dom'),
   }
  }
};
