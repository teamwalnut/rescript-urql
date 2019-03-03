const path = require("path");

module.exports = {
  entry: path.join(__dirname, "examples/1-basic/app/Basic.bs.js"),
  mode: "development",
  output: {
    path: path.resolve(__dirname, "examples/build"),
    publicPath: "/public/",
    filename: "Index.js"
  },
  devServer: {
    open: true,
    contentBase: path.resolve(__dirname, "examples/public")
  },
  module: {
    rules: [
      {
        test: /\.svg$/,
        loader: "url-loader"
      }
    ]
  }
};
