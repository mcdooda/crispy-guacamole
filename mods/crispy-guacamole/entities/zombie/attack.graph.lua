script {
  links = {
    {
      1,
      1,
      3,
      2,
    },
    {
      1,
      2,
      7,
      3,
    },
    {
      2,
      1,
      3,
      4,
    },
    {
      3,
      2,
      5,
      1,
    },
    {
      3,
      3,
      9,
      3,
    },
    {
      3,
      4,
      7,
      4,
    },
    {
      4,
      1,
      3,
      3,
    },
    {
      5,
      1,
      7,
      2,
    },
    {
      6,
      1,
      5,
      2,
    },
    {
      8,
      1,
      7,
      1,
    },
  },
  nodes = {
    {
      name = 'script/scriptinput',
      initArguments = {
        {
          4,
          'Number',
        },
        {
          5,
          'String',
        },
      },
    },
    {
      name = 'init',
    },
    {
      name = 'test',
    },
    {
      name = 'string',
      initArguments = {
        'Hello world from saved script',
      },
    },
    {
      name = 'math/multiply',
    },
    {
      name = 'number',
      initArguments = {
        2,
      },
    },
    {
      name = 'test',
    },
    {
      name = 'bool',
    },
    {
      name = 'test',
    },
  },
}