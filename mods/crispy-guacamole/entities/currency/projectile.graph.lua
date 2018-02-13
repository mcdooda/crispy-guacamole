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
      10,
      1,
    },
    {
      1,
      3,
      5,
      1,
    },
    {
      1,
      3,
      13,
      2,
    },
    {
      1,
      3,
      14,
      2,
    },
    {
      1,
      5,
      7,
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
      7,
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
      8,
      1,
    },
    {
      8,
      1,
      6,
      1,
    },
    {
      8,
      1,
      13,
      3,
    },
    {
      9,
      1,
      8,
      2,
    },
    {
      10,
      1,
      14,
      1,
    },
    {
      10,
      2,
      13,
      1,
    },
    {
      11,
      1,
      10,
      2,
    },
    {
      12,
      1,
      11,
      2,
    },
  },
  nodes = {
    {
      name = 'components/projectilecomponent',
    },
    {
      loadArguments = {
        60,
      },
      name = 'number',
    },
    {
      loadArguments = {
        16,
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
      name = 'entity/getprojectilespeed',
    },
    {
      name = 'math/vector3/vector3length',
    },
    {
      name = 'math/vector3/vector3reflect',
    },
    {
      name = 'math/vector3/vector3numbermultiply',
    },
    {
      loadArguments = {
        0.80000001192093,
      },
      name = 'number',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'math/lessthan',
    },
    {
      loadArguments = {
        3,
      },
      name = 'number',
    },
    {
      name = 'entity/setprojectilespeed',
    },
    {
      name = 'entity/despawnentity',
    },
  },
}