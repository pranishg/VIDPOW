#!/usr/bin/env python3

import argparse
import json
import os
import sys

CHANGELOG_PLUGIN = False

def find_plugin_filenames(basedir):
    for root, dirs, files in os.walk(basedir):
        dirs_to_remove = [d for d in dirs if d.startswith(".")]
        for d in dirs_to_remove:
            dirs.remove(d)
        if "plugin.json" in files:
            plugin_filename = os.path.join(root, "plugin.json")
            if not CHANGELOG_PLUGIN:
                if not plugin_filename.endswith('/plugins/changelog/plugin.json'):
                    yield plugin_filename
            else:
                yield plugin_filename


def find_plugins(basedir):
    for plugin_json in find_plugin_filenames(basedir):
        with open(plugin_json, "r") as f:
            yield json.load(f)


def process_plugin(ctx, plugin):
    ctx["plugins"].append(plugin)
    for iext in plugin.get("index_extensions", []):
        ctx["index_extensions"].append(iext)
    return


def main(argv):

    parser = argparse.ArgumentParser( description="Find plugins" )

    parser.add_argument("plugin_dir", metavar="DIR", type=str, nargs="+",
                        help="Dir(s) to search for templates")
    parser.add_argument("-o", "--output", metavar="FILE", type=str, default="-",
                        help="Output file")
    parser.add_argument( "--print-dependencies", action="store_true", help="Print dependencies and exit")

    parser.add_argument( "--CHANGELOG_PLUGIN", required=False, action="store_true", help="changelog plugin enable")

    args = parser.parse_args()


    global CHANGELOG_PLUGIN
    CHANGELOG_PLUGIN = args.CHANGELOG_PLUGIN

    if args.print_dependencies:
        deps = []
        for d in args.plugin_dir:
            for plugin in find_plugin_filenames(d):
                deps.append(plugin)

        # if args.CHANGELOG_PLUGIN == False:
        #     for plugin_filename in deps:
        #         if plugin_filename.endswith('/plugins/changelog/plugin.json'):
        #             deps.remove(plugin_filename)

        print(";".join(deps))
        return

    if args.output == "-":
        outfile = sys.stdout
    else:
        os.makedirs(os.path.dirname(args.output), exist_ok=True)
        outfile = open(args.output, "w")

    ctx = {
        "plugins" : [],
        "index_extensions" : [],
        }
    for d in args.plugin_dir:
        for plugin in find_plugins(d):
            process_plugin(ctx, plugin)
    outfile.write(json.dumps(ctx))
    outfile.write("\n")
    outfile.close()

if __name__ == "__main__":
    main(sys.argv)
