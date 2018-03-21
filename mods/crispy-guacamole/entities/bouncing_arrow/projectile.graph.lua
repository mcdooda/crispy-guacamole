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
      6,
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
      21,
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
        15,
      },
      name = 'number',
    },
    {
      loadArguments = {
        5,
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
        'fx_explosion',
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
        0.89999997615814,
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
        true,
      },
      name = 'boolean',
    },
  },
}