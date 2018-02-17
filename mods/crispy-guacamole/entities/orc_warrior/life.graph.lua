script {
  links = {
    {
      1,
      1,
      3,
      1,
    },
    {
      1,
      3,
      4,
      1,
    },
    {
      1,
      4,
      4,
      2,
    },
    {
      2,
      1,
      1,
      1,
    },
    {
      5,
      1,
      4,
      3,
    },
    {
      6,
      1,
      4,
      4,
    },
    {
      7,
      1,
      4,
      5,
    },
  },
  nodes = {
    {
      name = 'components/lifecomponent',
    },
    {
      loadArguments = {
        1500,
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
      name = 'entity/playanimation',
    },
    {
      loadArguments = {
        'death',
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
      name = 'bool',
    },
  },
}