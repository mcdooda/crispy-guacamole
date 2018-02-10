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
      11,
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
  },
}