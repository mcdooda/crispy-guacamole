script {
  links = {
    {
      1,
      1,
      5,
      1,
    },
    {
      1,
      2,
      6,
      1,
    },
    {
      1,
      3,
      6,
      2,
    },
    {
      2,
      1,
      1,
      1,
    },
    {
      3,
      1,
      1,
      2,
    },
    {
      4,
      1,
      1,
      3,
    },
  },
  nodes = {
    {
      name = 'components/projectilecomponent',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      loadArguments = {
        25,
      },
      name = 'number',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      name = 'entity/despawnentity',
    },
  },
}