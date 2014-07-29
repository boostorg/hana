This directory contains the `gh-pages` branch of the Boost.Hana repository
as a submodule. To be able to browse the most recent documentation offline,
you must either issue the following command from the directory where this
file is located:

```shell
git pull origin gh-pages
```

or issue the following command from the root of the main Boost.Hana repository:

```shell
git submodule update --init --remote
```

Both of these commands will update this submodule with the latest version
of the documentation. Note that the main Boost.Hana repository will then
appear to have uncommitted changes because the main repository will still
be pointing to an earlier version of this submodule; just ignore it.
