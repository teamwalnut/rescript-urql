const path = require("path");

module.exports = {
  entry: path.join(__dirname, "src/index.bs.js"),
  mode: "development",
  output: {
    path: path.resolve(__dirname, "build"),
    publicPath: "/public/",
    filename: "index.js"
  },
  resolve: {
    modules: [path.resolve(__dirname, "../../node_modules"), "node_modules"]
  },
  devServer: {
    open: true,
    contentBase: path.resolve(__dirname, "public")
  }
};
