script {
  nodes = {
    {
      name = 'components/projectilecomponent',
    },
    {
      name = 'number',
      loadArguments = {
        30,
      },
    },
    {
      name = 'number',
      loadArguments = {
        12,
      },
    },
    {
      name = 'script/scriptoutput',
      loadArguments = {
        {
          7,
          'Table',
        },
      },
    },
    {
      name = 'script/condition',
    },
    {
      name = 'entity/spawnentity',
    },
    {
      name = 'string',
      loadArguments = {
        'fx_explosion',
      },
    },
    {
      name = 'script/function',
    },
    {
      name = 'entity/getposition',
    },
    {
      name = 'entity/getheading',
    },
    {
      name = 'entity/despawnentity',
    },
    {
      name = 'entity/isvalidentity',
    },
  },
  links = {
    {
      1,
      1,
      4,
      1,
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
      5,
      2,
      6,
      1,
    },
    {
      6,
      1,
      11,
      1,
    },
    {
      7,
      1,
      6,
      2,
    },
    {
      8,
      1,
      1,
      3,
    },
    {
      8,
      2,
      5,
      1,
    },
    {
      8,
      3,
      11,
      2,
    },
    {
      8,
      3,
      9,
      1,
    },
    {
      8,
      3,
      10,
      1,
    },
    {
      8,
      4,
      12,
      1,
    },
    {
      9,
      1,
      6,
      3,
    },
    {
      10,
      1,
      6,
      4,
    },
    {
      12,
      1,
      5,
      2,
    },
  },
}