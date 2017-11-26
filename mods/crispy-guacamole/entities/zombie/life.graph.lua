script {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      3,
      1,
      1,
      1,
    },
    {
      4,
      1,
      1,
      2,
    },
    {
      4,
      2,
      5,
      1,
    },
    {
      4,
      3,
      5,
      2,
    },
    {
      6,
      1,
      5,
      3,
    },
    {
      7,
      1,
      5,
      4,
    },
    {
      8,
      1,
      5,
      5,
    },
    {
      9,
      1,
      1,
      3,
    },
    {
      9,
      2,
      10,
      1,
    },
    {
      9,
      3,
      10,
      2,
    },
    {
      11,
      1,
      10,
      3,
    },
    {
      12,
      1,
      10,
      4,
    },
    {
      13,
      1,
      10,
      5,
    },
  },
  nodes = {
    {
      name = 'components/lifecomponent',
    },
    {
      name = 'script/scriptoutput',
      loadArguments = {
        {
          5,
          'Table',
        },
      },
    },
    {
      name = 'number',
      loadArguments = {
        100,
      },
    },
    {
      name = 'script/function',
    },
    {
      name = 'entity/playanimation',
    },
    {
      name = 'string',
      loadArguments = {
        'spawn',
      },
    },
    {
      name = 'number',
      loadArguments = {
        1,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        true,
      },
    },
    {
      name = 'script/function',
    },
    {
      name = 'entity/playanimation',
    },
    {
      name = 'string',
      loadArguments = {
        'despawn',
      },
    },
    {
      name = 'number',
      loadArguments = {
        1,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        true,
      },
    },
  },
}