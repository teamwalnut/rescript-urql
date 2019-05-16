const path = require("path");

module.exports = {
  entry: path.join(__dirname, "src/app/index.bs.js"),
  mode: "development",
  output: {
    path: path.resolve(__dirname, "build"),
    publicPath: "/public/",
    filename: "index.js"
  },
  devServer: {
    open: true,
    contentBase: path.resolve(__dirname, "public")
  }
};
