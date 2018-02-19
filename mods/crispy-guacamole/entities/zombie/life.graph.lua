script {
  links = {
    {
      1,
      1,
      2,
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
      4,
      5,
      2,
    },
    {
      1,
      4,
      10,
      2,
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
      10,
      5,
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
      10,
      4,
    },
    {
      11,
      1,
      10,
      3,
    },
  },
  nodes = {
    {
      name = 'components/lifecomponent',
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
      loadArguments = {
        100,
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
      name = 'entity/playanimation',
    },
    {
      loadArguments = {
        'spawn',
      },
      name = 'string',
    },
    {
      loadArguments = {
        1,
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
        1,
      },
      name = 'number',
    },
    {
      name = 'entity/playanimation',
    },
    {
      loadArguments = {
        'despawn',
      },
      name = 'string',
    },
  },
}