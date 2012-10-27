# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant::Config.run do |config|
  config.vm.box = "base"

  config.vm.provision :chef_solo do |chef|
    chef.cookbooks_path = "cookbooks"
    chef.add_recipe "vagrant_main"
    # chef.add_recipe "mysql::client"
    # chef.add_recipe "mysql::server"
    # chef.json = {
    #   "mysql" => {
    #     "server_root_password" => "g0L1om@",
    #     "server_repl_password" => "g0L1om@",
    #     "server_debian_password" => "g0L1om@"
    #   }
    # }
    # chef.add_recipe "java"
    # chef.add_recipe "maven"
    chef.add_recipe "boost"
  end

  config.vm.forward_port 80, 12306
end
