script {
  links = {
    {
      1,
      1,
      4,
      1,
    },
    {
      1,
      2,
      5,
      1,
    },
    {
      1,
      3,
      19,
      2,
    },
    {
      1,
      3,
      11,
      2,
    },
    {
      1,
      3,
      25,
      1,
    },
    {
      1,
      3,
      27,
      1,
    },
    {
      1,
      3,
      29,
      2,
    },
    {
      1,
      4,
      8,
      1,
    },
    {
      1,
      4,
      22,
      1,
    },
    {
      1,
      4,
      13,
      2,
    },
    {
      1,
      4,
      9,
      1,
    },
    {
      1,
      4,
      27,
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
      5,
      1,
      28,
      1,
    },
    {
      5,
      2,
      18,
      1,
    },
    {
      6,
      1,
      19,
      1,
    },
    {
      7,
      1,
      18,
      2,
    },
    {
      8,
      1,
      5,
      2,
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
      1,
      3,
    },
    {
      14,
      1,
      13,
      3,
    },
    {
      15,
      1,
      13,
      4,
    },
    {
      17,
      1,
      6,
      2,
    },
    {
      18,
      1,
      11,
      1,
    },
    {
      22,
      1,
      24,
      2,
    },
    {
      23,
      1,
      22,
      2,
    },
    {
      24,
      1,
      13,
      1,
    },
    {
      25,
      1,
      18,
      3,
    },
    {
      26,
      1,
      18,
      4,
    },
    {
      27,
      1,
      28,
      2,
    },
    {
      28,
      1,
      6,
      1,
    },
    {
      28,
      1,
      24,
      1,
    },
    {
      28,
      2,
      29,
      1,
    },
  },
  nodes = {
    {
      name = 'components/projectilecomponent',
    },
    {
      loadArguments = {
        30,
      },
      name = 'number',
    },
    {
      loadArguments = {
        10,
      },
      name = 'number',
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
      name = 'script/condition',
    },
    {
      name = 'entity/spawnentity',
    },
    {
      loadArguments = {
        'fx_dust',
      },
      name = 'string',
    },
    {
      name = 'entity/isvalidentity',
    },
    {
      name = 'entity/getposition',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      name = 'entity/despawnentity',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      name = 'entity/dealdamage',
    },
    {
      loadArguments = {
        49,
      },
      name = 'number',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'fx_splatter',
      },
      name = 'string',
    },
    {
      name = 'entity/spawnentity',
    },
    {
      name = 'entity/despawnentity',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      name = 'entity/hascomponent',
    },
    {
      loadArguments = {
        'life',
      },
      name = 'string',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'entity/getposition',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      name = 'entity/ishostileentity',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'entity/despawnentity',
    },
  },
}