#
#   XFireDB commands
#   Copyright (C) 2015  Michel Megens <dev@michelmegens.net>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

module XFireDB
  class Command
    @cmd = nil
    @argv = nil
    @raw = nil
    @cluster = nil
    @client = nil

    def initialize(cluster, cmd, client, raw = nil)
      @cluster = cluster
      @cmd = cmd
      @argv = client.request.args
      @raw = raw
      @client = client

      unless @cmd == "AUTH"
        raise IllegalKeyException, "Key: #{@argv[0]} is illegal" if XFireDB.illegal_key? @argv[0]
      end
    end

    def forward(key, query = nil)
      node = @cluster.where_is? key
      node = @cluster.nodes[node]
      return node.query @client, @raw unless @raw.nil?
      return node.query @client, query
    end
  end
end

