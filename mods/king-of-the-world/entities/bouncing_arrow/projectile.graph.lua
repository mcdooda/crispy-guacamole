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
      10,
      1,
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
      9,
      1,
    },
    {
      1,
      3,
      12,
      1,
    },
    {
      1,
      3,
      20,
      2,
    },
    {
      1,
      3,
      24,
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
      5,
      13,
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
      24,
      1,
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
      18,
      1,
    },
    {
      7,
      1,
      1,
      3,
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
      9,
      1,
      22,
      3,
    },
    {
      9,
      1,
      25,
      3,
    },
    {
      10,
      1,
      6,
      4,
    },
    {
      10,
      1,
      22,
      4,
    },
    {
      10,
      1,
      25,
      4,
    },
    {
      12,
      1,
      13,
      1,
    },
    {
      13,
      1,
      14,
      1,
    },
    {
      14,
      1,
      11,
      3,
    },
    {
      14,
      1,
      16,
      1,
    },
    {
      15,
      1,
      14,
      2,
    },
    {
      16,
      1,
      17,
      1,
    },
    {
      17,
      1,
      18,
      2,
    },
    {
      18,
      1,
      20,
      1,
    },
    {
      18,
      2,
      11,
      1,
    },
    {
      19,
      1,
      17,
      2,
    },
    {
      20,
      1,
      22,
      1,
    },
    {
      21,
      1,
      6,
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
      25,
      1,
    },
    {
      26,
      1,
      25,
      2,
    },
  },
  nodes = {
    {
      name = 'components/projectilecomponent',
    },
    {
      loadArguments = {
        39,
      },
      name = 'number',
    },
    {
      loadArguments = {
        31,
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
        false,
      },
      name = 'boolean',
    },
    {
      name = 'entity/isvalidentity',
    },
    {
      name = 'entity/getposition',
    },
    {
      name = 'entity/getheading',
    },
    {
      name = 'entity/setprojectilespeed',
    },
    {
      name = 'entity/getprojectilespeed',
    },
    {
      name = 'math/vector3/vector3reflect',
    },
    {
      name = 'math/vector3/vector3numbermultiply',
    },
    {
      loadArguments = {
        0.60000002384186,
      },
      name = 'number',
    },
    {
      name = 'math/vector3/vector3length',
    },
    {
      name = 'math/lessthan',
    },
    {
      name = 'script/condition',
    },
    {
      loadArguments = {
        5,
      },
      name = 'number',
    },
    {
      name = 'entity/despawnentity',
    },
    {
      loadArguments = {
        'fx_dust',
      },
      name = 'string',
    },
    {
      name = 'entity/spawnentity',
    },
    {
      loadArguments = {
        'fx_explosion2',
      },
      name = 'string',
    },
    {
      name = 'entity/despawnentity',
    },
    {
      name = 'entity/spawnentity',
    },
    {
      loadArguments = {
        'fx_explosion2',
      },
      name = 'string',
    },
  },
}