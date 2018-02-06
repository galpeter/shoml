var iotjs_basedir = process.argv[0].split('/').slice(0, -1).join('/');
var iotjs_bin_path = process.cwd() + '/' + iotjs_basedir;


var so_name = "libiotjs_dynmodule.so";
var target_so_path = iotjs_bin_path + '/../lib/' + so_name;

console.log("Loading module from: " + target_so_path);

var shoml = require('shoml');
var loaded_module = shoml.loadso(target_so_path);

console.log("Module properties: " + Object.keys(loaded_module));

console.log(loaded_module.message + " " + loaded_module.sharedInfo);
